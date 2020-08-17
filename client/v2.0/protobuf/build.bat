call build_setup.bat

protogen quantmodel.protobin
%NANT_HOME%\nant.exe %1 %2 %3 %4 %5 %6 %7 %8 %9

@echo on