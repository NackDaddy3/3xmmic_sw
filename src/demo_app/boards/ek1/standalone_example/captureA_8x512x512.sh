#!/bin/bash

# ------------------------------------------------------------------------------
# This debug script polls CSI-2 radar data according the the v4l2-ctl parameters
# This script has to be executed after the CTRX is sent to operational mode, otherwise
# the Jetson receiver won't turn on the receiver/lane termination because of missing CSI2 clock.
# After capturing the CSI-2 data, the data is dumped to STDOUT and two files
# are created inside the output folder:
#
#   1. The captured data in raw binary format
#   2. The hex-dumped packed (padded) data
#
# The padding applied by NVIDIA for RAW-12 appends 4 redundant bits after
# 12 data bits to round the 12 bits up to 16. Check the NVIDIA technical 
# reference document for more information on padding.
# ------------------------------------------------------------------------------

OUTPUT_FOLDER="."

#rm -rf "$OUTPUT_FOLDER"
#mkdir -p "$OUTPUT_FOLDER"

# Double-check that the dev-node matches your intent
echo "[INFO] Polling radar data"
v4l2-ctl -d /dev/video0 \
    --set-fmt-video=width=4096,height=512,pixelformat=RG12 \
    --set-ctrl bypass_mode=0 \
    --stream-mmap \
    --stream-count=1 \
    --stream-to=$OUTPUT_FOLDER/ctrx1_bin.raw 
        
EXIT_VALUE=$?
if [ $EXIT_VALUE -ne 0 ]; then
    echo "[ERROR] v4l2-ctl returned a non-zero exit status"
    exit $EXIT_VALUE
fi

echo "[INFO] Dumping and displaying unpacked radar data"
hexdump $OUTPUT_FOLDER/ctrx1_bin.raw > $OUTPUT_FOLDER/ctrx1_hex_packed.txt
cat $OUTPUT_FOLDER/ctrx1_hex_packed.txt
sleep 30  # Added if called from run.sh script to keep terminal alive
