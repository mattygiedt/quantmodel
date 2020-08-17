#! /bin/sh

perl create_cpp_procs.pl
perl create_cs_procs.pl
perl create_procs.pl
cp QuantModelProcedures.hpp ../../core/cpp/
cp QuantModelProcedures.cs /home/mgiedt/quantmodel/source/client/v2.0/common/com/quantmodel/common
