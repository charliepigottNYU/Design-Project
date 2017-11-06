To build:
    currently you must have golang, g++ and postgres psql all installed to run soundwave properly
    psql must be in the system path to connect to the database properly
    Soundwave builds are currently only compatible with unix based operating systems
    The frontnend website side of soundwave is currently compatible with any modern web browser

    1. navigate to ./src/rest
    2. enter "go build"
    3. navigate to ./src/src
    4. enter "make"

To run:
    1. navigate to ./src/rest
    2. run ./rest
    3. navigate to ./src/src
    4. run ./soundwave_server
