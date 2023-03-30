P=1
T=$(PWD)/$(P)

debug: mkbd
	gcc -o $(T)/build/main $(T)/main.c -std=c11 -g

final: mkbd
	gcc -o $(T)/build/main $(T)/main.c -std=c11 -O2 -static

clean: 
	rm -d -rf **/build

mkbd:
	mkdir $(T)/build || echo 'build dir exist'