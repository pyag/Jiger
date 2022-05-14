sfmlPath := {Path where you downloaded SFML}
# example:
# sfmlPath := /home/user/SFML-2.5

compile:
	g++ -c main.cpp GlobalConfigs.cpp Div.cpp DataNode.cpp Parser.cpp EditorSpace.cpp Tab.cpp TabTray.cpp Explorer.cpp FileOperations.cpp Buffer.cpp langHighlight/*.cpp langHighlight/cpp/*.cpp -std=c++11
	g++ main.o GlobalConfigs.o Div.o DataNode.o Parser.o EditorSpace.o Tab.o TabTray.o Explorer.o FileOperations.o Buffer.o highlight.o keyword.o -o jiger -lsfml-graphics -lsfml-window -lsfml-system

run:
	./jiger

src := source

compile2.5:
	g++ -c main.cpp ${src}/GlobalConfigs.cpp ${src}/Div.cpp ${src}/DataNode.cpp ${src}/Parser.cpp ${src}/EditorSpace.cpp ${src}/Tab.cpp ${src}/TabTray.cpp ${src}/Explorer.cpp ${src}/FileOperations.cpp ${src}/Buffer.cpp langHighlight/*.cpp langHighlight/cpp/*.cpp -I${sfmlPath}/include -std=c++11
	g++ main.o GlobalConfigs.o Div.o DataNode.o Parser.o EditorSpace.o Tab.o TabTray.o Explorer.o FileOperations.o Buffer.o highlight.o keyword.o -o jiger -L${sfmlPath}/lib -lsfml-graphics -lsfml-window -lsfml-system

	rm -f *.o

run2.5:
	export LD_LIBRARY_PATH=${sfmlPath}/lib && ./jiger

clean:
	rm -f *.o
	rm -f jiger
