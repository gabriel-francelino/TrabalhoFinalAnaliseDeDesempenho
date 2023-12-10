make:
	g++ -o main main.cpp fila.cpp;
	./main;

texto:
	g++ -o main main.cpp fila.cpp;
	rm saida.txt;
	./main > saida.txt;

clean:
	rm main saida.txt;