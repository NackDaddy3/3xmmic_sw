#!/bin/bash

# ------------------------------------------------------------------------------
# This debug script polls CSI-2 radar data according the the v4l2-ctl parameters
# This script has to be executed after the CTRX is sent to operational mode, otherwise
# the Jetson receiver won't turn on the receiver/lane termination because of missing CSI2 clock.
# After capturing the CSI-2 data, the data is dumped to STDOUT and two files for each video device
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
DEFAULT_WIDTH=4096

#rm -rf "$OUTPUT_FOLDER"
#mkdir -p "$OUTPUT_FOLDER"

# Configuration for video devices
VIDEO0_WIDTH=$DEFAULT_WIDTH
VIDEO1_WIDTH=$DEFAULT_WIDTH
VIDEO2_WIDTH=$DEFAULT_WIDTH # Half width

# Function to run v4l2-ctl for a specific video device
run_v4l2_capture() {
    local device=$1
    local width=$2
    local output_file=$3
    
    echo "[INFO] Starting capture for $device with width $width"
    v4l2-ctl -d $device \
        --set-fmt-video=width=$width,height=512,pixelformat=RG12 \
        --set-ctrl bypass_mode=0 \
        --stream-mmap \
        --stream-count=1 \
        --stream-to=$output_file
    
    local exit_value=$?
    if [ $exit_value -ne 0 ]; then
        echo "[ERROR] v4l2-ctl for $device returned exit status $exit_value"
    else
        echo "[INFO] v4l2-ctl for $device completed successfully"
    fi
    
    return $exit_value
}

# Double-check that the dev-nodes match your intent
echo "[INFO] Starting parallel radar data polling for 4 video devices"

# Start v4l2-ctl instances in parallel
run_v4l2_capture /dev/video0 $VIDEO0_WIDTH $OUTPUT_FOLDER/ctrx0_bin.raw &
PID0=$!

run_v4l2_capture /dev/video1 $VIDEO1_WIDTH $OUTPUT_FOLDER/ctrx1_bin.raw &
PID1=$!

run_v4l2_capture /dev/video2 $VIDEO2_WIDTH $OUTPUT_FOLDER/ctrx2_bin.raw &
PID2=$!

# Wait for all processes to complete and log when each finishes
PIDS=($PID0 $PID1 $PID2)
DEVICES=("video0" "video1" "video2")
EXIT_VALUES=(0 0 0)

for i in ${!PIDS[@]}; do
    wait ${PIDS[$i]}
    EXIT_VALUES[$i]=$?
    echo "[INFO] ${DEVICES[$i]} capture finished with exit code ${EXIT_VALUES[$i]}"
done

# Check if any process failed
OVERALL_EXIT=0
for i in ${!EXIT_VALUES[@]}; do
    if [ ${EXIT_VALUES[$i]} -ne 0 ]; then
        echo "[ERROR] ${DEVICES[$i]} failed with exit status ${EXIT_VALUES[$i]}"
        OVERALL_EXIT=${EXIT_VALUES[$i]}
    fi
done

if [ $OVERALL_EXIT -ne 0 ]; then
    echo "[ERROR] One or more v4l2-ctl instances failed"
    exit $OVERALL_EXIT
fi

echo "[INFO] All captures completed successfully"

echo "[INFO] Dumping and displaying unpacked radar data"
for i in 0 1 2; do
    if [ -f "$OUTPUT_FOLDER/ctrx${i}_bin.raw" ]; then
        hexdump $OUTPUT_FOLDER/ctrx${i}_bin.raw > $OUTPUT_FOLDER/ctrx${i}_hex_packed.txt
        echo "[INFO] Generated hex dump for ctrx${i}_bin.raw"
    fi
done

sleep 30  # Added if called from run.sh script to keep terminal alive
