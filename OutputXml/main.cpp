#include <fstream>
#include <sstream>
#include <iosfwd>

int main(){
	const int STEPS = 2000;
	std::string prefix = "../ShallowWater-Box/";
	for (int step = 0; step < STEPS; step++){
		std::string templatefn("result/frame.xml");
		std::ifstream xmltemplate(prefix + templatefn);
		if (!xmltemplate.is_open()){
			perror("output error");
			exit(-1);
		}

		std::stringstream templatebuffer;
		templatebuffer << xmltemplate.rdbuf();
		std::string contents(templatebuffer.str());
		std::stringstream stepbuffer;;
		std::string source("STEP");
		std::string target;
		stepbuffer << step;
		stepbuffer >> target;
		int pos = contents.find(source, 0);
		if (pos == std::string::npos)
		{
			perror("template error");
			exit(-2);
		}
		contents.replace(pos, source.size(), target);
		xmltemplate.close();

		std::stringstream xmlbuffer;
		std::string xmlfn;
		xmlbuffer <<prefix<< "result/obj/frame_" << step << ".xml";
		xmlbuffer >> xmlfn;
		std::ofstream xml(xmlfn);
		if (!xml.is_open()){
			perror("output error");
			exit(-1);
		}
		xml << contents;
		xml.close();
	}
}