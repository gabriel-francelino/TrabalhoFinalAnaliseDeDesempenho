make:
	g++ -o main main.cpp fila.cpp;
	./main;

compile:
	g++ -o main main.cpp fila.cpp;
	
texto:
	g++ -o main main.cpp fila.cpp;
	rm saida.txt;
	./main > saida.txt;

clean:
	rm main saida.txt tempo.txt;