all:
	chmod +x runme.sh
	chmod +x createAFile.sh
	gcc errorHandler.c main.c -o task
	./runme.sh
