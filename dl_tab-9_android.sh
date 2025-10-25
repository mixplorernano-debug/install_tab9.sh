#!/system/bin/sh
set -e

# This script downloads the TabNine binary for Android.

# Check if we have curl and unzip
if ! command -v curl > /dev/null; then
    echo "curl is not installed. Please install it first."
    exit 1
fi

if ! command -v unzip > /dev/null; then
    echo "unzip is not installed. Please install it first."
    exit 1
fi

# Get the latest version
version="$(curl -sS https://update.tabnine.com/bundles/version)"

# Set the target for Android (assuming ARM64)
target="aarch64-unknown-linux-musl"

# Set the download directory
download_dir="/sdcard/Download/tabnine"

# Remove old binaries
rm -rf "$download_dir"

# Create the directory structure
mkdir -p "$download_dir/$version/$target"

# Set the path
path="$version/$target"

echo "Downloading TabNine $version for Android ($target)"

# Download the binary
curl -sS "https://update.tabnine.com/bundles/$path/TabNine.zip" > "$download_dir/$path/TabNine.zip"

# Unzip the downloaded file
unzip -o "$download_dir/$path/TabNine.zip" -d "$download_dir/$path"

# Remove the zip file
rm "$download_dir/$path/TabNine.zip"

# Make the binary executable
chmod +x "$download_dir/$path/"*

echo "TabNine has been downloaded to $download_dir/$path"
echo "You may need to move it to a location in your PATH to use it system-wide."
To use this script:
1.
Save it as dl_tabnine_android.sh in your Android device's Download folder.
2.
Open a terminal emulator on your Android device.
3.
Navigate to the Download folder:
cd /sdcard/Download
4.
Make the script executable:
chmod +x dl_tabnine_android.sh
5.
Run the script:
./dl_tabnine_android.sh
Please note:
This script assumes you're using an ARM64 Android device. If you're using a different architecture, you'll need to change the target variable accordingly.
The script downloads the TabNine binary to the /sdcard/Download/tabnine directory. You may need to move it to a location in your PATH to use it system-wide.
Running shell scripts and using command-line tools on Android can be tricky and may require root access or special permissions. Make sure you have the necessary permissions and understand the implications of running such scripts on your device.
This script only downloads the binary for Android, unlike the original script which 
downloaded for multiple platforms.