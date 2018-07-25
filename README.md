# I2C tool

read and write data through i2c

## Compile

## Usage
i2ctool -i 2 -a 0x0f -r 2 -d 4  0x1122 0x11223344
	-i: i2c adapter bus index
	-a: i2c device address
	-r: register byte width
	-d: data byte width

	-n: read number, default 1
