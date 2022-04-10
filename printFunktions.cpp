#include "printFunktions.hpp"

using namespace std;

void Set_Console_Color(int color){
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#else
	switch (color)
	{
	case 14: // yellow
		printf("\x1B[93m");
		break;
	case 7: // white
		printf("\x1B[37m");
		break;
	case 4: // dark red
		printf("\x1B[31m");
		break;
	case 2: // dark green
		printf("\x1B[32m");
		break;
	default:
		break;
	}
#endif
}

void print_warning(string output){
    Set_Console_Color(14);
    cout << "Warning: " << output << endl;
    Set_Console_Color(7);
}

void print_error(string output){
    Set_Console_Color(4);
    cout << "Error: " << output << endl;
    Set_Console_Color(7);
}

void print_Debug(string output, int format){
    string prefix = "";
    switch (format) {
        case 1:
            Set_Console_Color(2);
            break;
        case 2:
            prefix = "Warning: ";
            Set_Console_Color(14);
            break;
        case 3:
            prefix = "Error: ";
            Set_Console_Color(4);
            break;
    }
    cout << prefix << output << endl;
    Set_Console_Color(7);
}

void openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_NOTIFICATION: {
		//std::cout << "[OpenGL Info] " << message << std::endl;
		break;
	}

	case GL_DEBUG_SEVERITY_HIGH: {
		print_Debug(message, 3);
		//std::cout << "[OpenGL ERROR] " << message << std::endl;
		break;
	}
	case GL_DEBUG_SEVERITY_MEDIUM: {
		print_Debug(message, 2);
		//std::cout << "[OpenGL WARNING] " << message << std::endl;
		break;
	}
	case GL_DEBUG_SEVERITY_LOW: {
		std::cout << "[OpenGL Warning] " << message << std::endl;
		break;
	}

	default:
		break;
	}

}





void slowPrint(string output) {
	static bool initialized = false;
	static int tick_counter;
	//static clock_t last_time;
	//clock_t current_time = clock();
	//clock_t difference = 0;
	tick_counter++;

	if (tick_counter >= 120) {
		cout << output << endl;
		tick_counter = 0;
	}
}

std::string vec3_toString(glm::vec3 vec, std::string prefix,int precision) {
	std::stringstream ss;
	string output;
	ss << prefix << ": " << std::fixed << std::setprecision(precision);
	ss << "x " <<  vec.x << ", ";
	ss << "y " << vec.y << ", ";
	ss << "z " << vec.z << ", ";
	output = ss.str();
	return output;
}

std::string posVector_toString(std::vector<glm::vec3> vector, std::string prefix) {
	std::stringstream ss;
	string output;
	if(prefix != ""){
		ss << prefix << ": " << std::endl;
	}
	for (int i = 0; i < vector.size(); i++) {
		ss << vec3_toString(vector.at(i)) << std::endl;
	}
	output = ss.str();
	return output;
}

//class LogStream
//{
//	std::ostream* myDest;
//public:
//	LogStream(std::ostream* dest) : myDest(dest) {}
//
//	template <typename T>
//	LogStream& operator<<(T const& obj)
//	{
//		if (myDest != nullptr) {
//			*myDest << obj;
//		}
//		return *this;
//	}
//};

