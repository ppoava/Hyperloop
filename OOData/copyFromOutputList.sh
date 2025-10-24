#!/bin/bash
# Usage: ./CopyFromGrid.sh <local_output_dir> <input_list_file> [maxgb]

LOCAL_DIR=$1
INPUT_LIST=$2
MAX_GB=$3  # optional, maximum GB to copy

if [[ -z "$LOCAL_DIR" || -z "$INPUT_LIST" ]]; then
  echo "Usage: ./CopyFromGrid.sh <local_output_dir> <input_list_file> [maxgb]"
  exit 1
fi

# Check if alien token is valid
if ! alien-token-info >/dev/null 2>&1; then
  echo "Alien token missing or expired. Please authenticate with:"
  echo "   alien.py whoami"
  exit 1
fi

mkdir -p "$LOCAL_DIR"

echo "Starting copy using file list: $INPUT_LIST"
echo "Local destination: $LOCAL_DIR"
if [[ -n "$MAX_GB" ]]; then
  echo "Maximum size allowed: $MAX_GB GB"
fi

while IFS= read -r FILEPATH; do
  [[ -z "$FILEPATH" ]] && continue
  FILEPATH=${FILEPATH#alien://}
  DIRPATH=$(dirname "$FILEPATH")
  BASENAME=$(echo "$DIRPATH" | sed 's#/#_#g' | cut -c2-)
  LOCAL_PATH="$LOCAL_DIR/$BASENAME"
  mkdir -p "$LOCAL_PATH"

  echo "Copying from: alien:$FILEPATH"
  echo "To:           $LOCAL_PATH"

  alien_cp "alien:$FILEPATH" "file:$LOCAL_PATH/$(basename "$FILEPATH")"

  # Print current total size
  CURRENT_SIZE=$(du -s "$LOCAL_DIR" | awk '{print $1}') # in KB
  CURRENT_GB=$(awk "BEGIN {printf \"%.2f\", $CURRENT_SIZE/1024/1024}")
  echo "Current total size of $LOCAL_DIR: $CURRENT_GB GB"
  echo "-----------------------------------------------"

  # Check max size
  if [[ -n "$MAX_GB" && $(echo "$CURRENT_GB >= $MAX_GB" | bc) -eq 1 ]]; then
    echo "Reached maximum allowed size of $MAX_GB GB. Stopping script."
    exit 0
  fi

done < "$INPUT_LIST"

echo "Yay. All copies finished."