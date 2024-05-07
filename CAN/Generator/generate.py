import datetime
import argparse
import sys
from pathlib import Path
from jinja2 import Environment, FileSystemLoader
from matrix import canmatrix
import util
import control_unit
import settings

headers = []
buses = set()
databases = dict()

data = {
    'datetime': f'{datetime.datetime.now():%d.%m.%Y %H:%M}',
    'hex': hex,
    'str': str,
    'util': util,
    'db_wrappers': {},
    'settings': settings,
    'units': control_unit.units,
    'headers': headers,
    'buses': buses,
    'databases': databases
}

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Generate C++ code for CAN')
    parser.add_argument('-s', '--sources', default='sources', type=str,
                        help='Output directory for generated code source files', dest='sources', metavar='directory')
    parser.add_argument('-c', '--headers', default='include', type=str,
                        help='Output directory for include headers', dest='headers', metavar='directory')
    parser.add_argument('-i', '--in', required=True, type=str, help='Input .dbc files in the correct order',
                        dest='input', nargs='+', metavar='file')

    args = parser.parse_args()
    template_directory = Path(__file__).parent.absolute() / 'template'
    env = Environment(loader=FileSystemLoader(searchpath=template_directory), autoescape=None)
    definitions_template = env.get_template('definitions.jinja2')
    declarations_template = env.get_template('declarations.jinja2')
    ecu_messages_template = env.get_template('ecu_messages.jinja2')
    include_header_template = env.get_template('include_header.jinja2')
    task_declaration_template = env.get_template('task_declaration.jinja2')
    task_definition_template = env.get_template('task_definition.jinja2')

    sources_dir = Path(args.sources)
    sources_dir.mkdir(parents=True, exist_ok=True)
    headers_dir = Path(args.headers)
    headers_dir.mkdir(parents=True, exist_ok=True)

    for _, dbc_file in enumerate(args.input):
        dbc_path = Path(dbc_file)
        db = canmatrix.formats.loadp(dbc_file)['']
        i = 0
        if "CANFD-Main" in dbc_file:
            i = 0
        elif "CAN-Front" in dbc_file or "CAN-Rear" in dbc_file:
            i = 1
        else:
            i = 2
        databases[str(i + settings.bus_start_index)] = db
        control_unit.load_from_db(db, i + settings.bus_start_index)

        dbcData = dict(data)
        dbcData['db'] = db
        dbcData['source_name'] = dbc_path.stem + '_dbc.cpp'
        dbcData['header_name'] = dbc_path.stem + '_dbc.h'
        headers.append(dbcData['header_name'])

        if 'GenMsgCycleTime' not in db.frame_defines:
            define = canmatrix.Define('')
            define.set_default('0')
            db.frame_defines['GenMsgCycleTime'] = define

        util.save_to_file(sources_dir / dbcData['source_name'], definitions_template.render(**dbcData))
        util.save_to_file(headers_dir / dbcData['header_name'], declarations_template.render(**dbcData))

    util.save_to_file(headers_dir / settings.ecu_header_name, ecu_messages_template.render(**data))
    util.save_to_file(headers_dir / settings.include_header_name, include_header_template.render(**data))

    util.save_to_file(headers_dir / (settings.task_filename + '.h'), task_declaration_template.render(**data))
    util.save_to_file(sources_dir / (settings.task_filename + '.cpp'), task_definition_template.render(**data))
