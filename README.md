# Project-Pico
My attempt at writing compression software

## How it Works

Pico uses frequency analysis to decrease the average number of bits required to express characters, thereby shortening the overall length of the file.

### Installation
```sh
cd Project-Pico
make
```

### Usage
```sh
# Basic example compression
pico .
# Compresses everything in current folder
# Outputs default compressed file name "compressed_file.pico"
```
By default, pico runs with configurations from "config.txt", but they can be replaced by using flags
```sh
# May compress files from multiple sources
pico file1name folder1name -name=compfiles.pico
# Outputs compressed file name "compfiles.pico"
```
```sh
# Decompression Example
pico -decompress compressed_filename.pico
# Decompresses and recreates the files and folders previously compressed
```

### Experimental Results

| Files | Uncompressed Size (MB) | Compressed Size (MB) |
| ----- | ---------------------- | -------------------- |
| Large Corpus | 11.2 | 6.0 |

### Coming Next
* More Experimental Results
* Better options for printing intermediary results
* Optional password protected files
* Write -makedefault flag
