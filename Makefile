
fixpermission:
	sudo chmod a+rw /dev/ttyUSB0

dependencies:
	arduino-cli core install arduino:avr

compile:
	arduino-cli compile --fqbn arduino:avr:uno game_project

upload:
	arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno game_project

buildrun: compile upload
