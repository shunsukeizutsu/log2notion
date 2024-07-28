#! /bin/bash

CMDNAME=`basename $0`
program_path=/home/izutsu330//gnss_plot
cmd_path="$program_path/bin"
log_path=$1

echo -n "Start plot gnss ..."
$cmd_path/plot_gnss -p $log_path -s -m 1
echo "OK"
