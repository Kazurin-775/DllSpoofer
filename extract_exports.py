import sys
import argparse
import pefile

parser = argparse.ArgumentParser(description='Extract exports from a DLL file.')
parser.add_argument('dllfile', type=str)
parser.add_argument('--use-noname', dest='NONAME_SUPPORTED',
    action='store_const', const=True, default=False)
args = parser.parse_args()

dll = pefile.PE(args.dllfile)
func_defs = open('func_defs.inc', 'w')
export_defs = open('exports.def', 'w')

def def_export(name, ord, unnamed):
    info = ''
    if ord:
        info += ' @%d' % ord
    if unnamed and args.NONAME_SUPPORTED:
        info += ' noname'
    return '%s = hook_%s%s\n' % (name, name, info)

if not hasattr(dll, 'DIRECTORY_ENTRY_EXPORT'):
    print('The PE file has no exports!')
    sys.exit(1)

func_defs.write('const int NUM_FUNCS = %d;\n' %
    len(dll.DIRECTORY_ENTRY_EXPORT.symbols));
func_defs.write('static DllFunc DLLFUNCS[NUM_FUNCS] = {\n');
export_defs.write('LIBRARY hook\nEXPORTS\n')
funcs = []

for sym in dll.DIRECTORY_ENTRY_EXPORT.symbols:
    if sym.name:
        sym_name = sym.name.decode('ASCII')
        func_defs.write('{ nullptr, "%s", %d },\n' % (sym_name, sym.ordinal))
        export_defs.write(def_export(sym_name, sym.ordinal, unnamed=False))
        funcs.append(sym_name)
    else:
        sym_name = 'unkExport%d' % sym.ordinal
        func_defs.write('{ nullptr, nullptr, %d },\n' % (sym.ordinal))
        export_defs.write(def_export(sym_name, sym.ordinal, unnamed=True))
        funcs.append(sym_name)

func_defs.write('};\n\n')

for index, func in enumerate(funcs):
    func_defs.write('FUNC(%s, %d)\n' % (func, index))

func_defs.close()
export_defs.close()
