(gci -include *.cs,*.cpp,*.h,*.idl,*.asmx -recurse | select-string .).Count
PAUSE