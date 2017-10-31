hp:
	@echo " Compile hp_main ...";
	gcc -I ./include/ -L ./lib/ -Wl,-rpath,./lib/ ./examples/hp_main.c ./src/heap_file.c -lbf -o ./build/runner -O2
	find -name data.db -print0 | xargs -0 -I file mv file ./last_data.db

bf:
	@echo " Compile bf_main ...";
	gcc -I ./include/ -L ./lib/ -Wl,-rpath,./lib/ ./examples/bf_main.c -lbf -o ./build/runner -O2
	find -name data.db -print0 | xargs -0 -I file mv file ./last_data.db
