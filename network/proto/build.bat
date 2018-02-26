pushd %~dp0

protoc -I=. --cpp_out=. 1.proto
