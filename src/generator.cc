#include "strict-generator.hh"

int main(int argc, char* argv[]) {
	StrictGenerator generator;
	PluginMain(argc, argv, &generator);
	return 0;
}
