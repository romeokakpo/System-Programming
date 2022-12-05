SRC1 = server.c
EXE1 = server

$(EXE1): $(SRC1)
	gcc $(SRC1) -o $(EXE1) -DPORT=6000 -DFILENAME=$(FILE)

run: $(EXE1)
	gnome-terminal -- ./$(EXE1) $(FILE)

clean:
	rm $(EXE1)