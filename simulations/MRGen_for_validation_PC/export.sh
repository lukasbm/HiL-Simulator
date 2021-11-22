#!/bin/bash
cd results

sed '/^.\{32765\}./d' MRGen_CAN_stream.vci > MRGen_CAN_stream_trimmed.vci
sed '/^.\{32765\}./d' MRGen_CAN_stream.vec > MRGen_CAN_stream_trimmed.vec
sed '/^.\{32765\}./d' MRGen_CAN_stream.sca > MRGen_CAN_stream_trimmed.sca

scavetool export *_trimmed.vec --filter "module(*acketSource*) OR module(*acketServer*) OR module(*ailQueue*) OR module(*Sink*) OR module(*eplayer*)" -o output.csv -F 'CSV-S'

