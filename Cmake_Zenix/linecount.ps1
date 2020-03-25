(gci -include *.cs,*.cpp,*.h,*.idl,*.asmx,*.py,*.hpp,*.ipp, CMakeLists.txt, *.cc -recurse | select-string .).Count
PAUSE