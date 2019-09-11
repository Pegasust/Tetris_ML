(gci -include *.cs,*.cpp,*.h,*.idl,*.asmx,*.py,*.hpp,*.ipp -recurse | select-string .).Count
PAUSE