#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <thread>
#include <cmath>
#include <sstream>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <climits>



namespace fs1 = boost::filesystem;

std::string get_current_directory(){
    char buf[PATH_MAX+1];
    getcwd(buf, sizeof(buf));
    return std::string(buf);
}

std::vector<std::string> vector_of_files_in_directory(){
    std::vector<std::string> dataOfVector;
    {
        std::string path1 = get_current_directory();
        for (auto & p : fs1::directory_iterator(path1))
            dataOfVector.emplace_back( p.path().string());
    }
    return dataOfVector;
}

std::vector<std::string> SplitStringToVector(std::string Name){
    std::vector<std::string> VectorOfResult;
    std::ifstream file (Name);
    if(!file){
        std::cout<<"File "<<Name<<" can not be opened!!!"<<std::endl;
    }
    else{
    std::string word;
    while( file >> word )
        VectorOfResult.push_back(word);}
    return VectorOfResult;
}


void WriteToFile(std::vector<std::string> &VectorOfWords, const std::string& NameOfFile) {
    std::ofstream fs(NameOfFile);
    for (auto &word : VectorOfWords) {
        fs <<word<<std::endl;
    }
}

void Enter(){
        std::cout<<"Please enter Y[es]/N[o]/A[ll]/C[ancel]"<<std::endl;
}


void Help(){
    std::cout <<">>> mycp [-h|--help] [-f] <file1> <file2>\n Means to copy from file1 to file2, if file1 is a directory, complete by reporting an error without copying.\n"
              <<">>> mycp [-h|--help] [-f]  <file1> <file2> <file3>... <dir>\n If at least one of the Xfiles is a directory, complete, reporting an error without copying.\n"
              <<">>> mycp [-h|--help] [-f] -R  <dir_or_file_1> <dir_or_file_2> <dir_or_file_3>... <dir> \n"" Recursively copy transmitted directories to <dir> \n"<< std::endl;
}

void change_directory(char *dirPATH){
    int erno1 = chdir(dirPATH);
    if (erno1 == -1){
        //std::cout <<"No such file in directory."<< std::endl;
        perror("Error");
        exit(erno1);
    }
}
void changeDirectory(std::string Directory){
    const char *cstr = Directory.c_str();
    change_directory((char *) cstr);
}
std::vector<std::string> Vector_of_all(std::string Directory){
    std::vector<std::string>Vector;
    changeDirectory(Directory);
    for(auto &p : fs1::recursive_directory_iterator(Directory))////////////////
    {
        Vector.emplace_back(p.path().string());
    }
    return Vector;
}

bool isFileExitInDirectory(std::string dir){
    fs1::path p{dir};
    bool key = fs1::is_regular_file(p);
    return key;

}

void WriteFileToDirectory(std::string directoryTo,std::string File){
    if (boost::filesystem::is_directory(directoryTo)) {//Is last file or dir?
            boost::filesystem::path dst_path = directoryTo + "/" + File;
            boost::filesystem::path src_path = File;
                try { boost::filesystem::copy_file(src_path, dst_path, boost::filesystem::copy_option::overwrite_if_exists);
                }
                catch (const boost::filesystem::filesystem_error &e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
        }
    }
    
std::vector<std::string> split_cmd_line(std::istream& is) {
    std::string commandline;
    std::vector<std::string> parsed_command;
    std::getline(is, commandline);
    std::stringstream ss(commandline);
    std::string word;
    while (ss >> word) {
        parsed_command.push_back(word);
    }
    return parsed_command;
}    

std::string getFromUser(){
    bool key = true;
    std::vector<std::string>Vector;
    while(key){
        Enter();
        Vector = split_cmd_line(std::cin);
        if(Vector[0] == "Y"|| Vector[0] == "y"|| Vector[0] == "N"|| Vector[0] == "n"|| Vector[0] == "A"|| Vector[0] == "a" || Vector[0] == "C"|| Vector[0] == "c"){
            key = false;   
        }
    }
    return Vector[0];  
}    

void m_coopy(fs1::path pathFrom,fs1::path pathTo){
    try { boost::filesystem::copy_file(pathFrom, pathTo, boost::filesystem::copy_option::overwrite_if_exists);
        }
        catch (const boost::filesystem::filesystem_error &e){
            //std::cerr << "Error: " << e.what() << std::endl;
            }        
}


bool CoopyFileToDirectory(fs1::path pathFrom,fs1::path pathTo,bool keyf, bool keyAll){
    //bool keyAll = false;
    if(keyf){
        if(pathFrom.parent_path().empty()){
            boost::filesystem::path dst_path = pathTo /pathFrom.filename().string();
            boost::filesystem::path src_path = get_current_directory() + "/" + pathFrom.filename().string();
            m_coopy(src_path,dst_path);
        }
        else{
            boost::filesystem::path pathTo2 =  pathTo /pathFrom.filename().string();
            m_coopy(pathFrom,pathTo2);
        }
    }
    else{
        if(fs1::is_regular_file(pathTo /pathFrom.filename().string())){
            //std::cout<<pathFrom<<std::endl;
            std::string simbol = getFromUser();
         ////////////////////////////////////////////////!!!!!!!!!!!!!!!!!!!
            if(simbol == "Y" || simbol == "y"){                
                CoopyFileToDirectory(pathFrom,pathTo, not keyf,keyAll);
            }
            else if(simbol == "C" || simbol == "c"){
                exit(0);
            }
            else if(simbol == "a" || simbol == "A"){
                keyAll = true;
            }
        }
        else{
            CoopyFileToDirectory(pathFrom,pathTo, not keyf,keyAll);
        }
    }
    return keyAll;
}

fs1::path FileFinder(std::string Name){
    fs1::path PathToFile = Name;
    fs1::path Path;
    if (PathToFile.parent_path().empty()) {
        Path = get_current_directory() + "/" + PathToFile.filename().string();
    } else {
        Path = PathToFile;
    }
    if (fs1::is_regular_file(Path)) {
        changeDirectory(Path.parent_path().string());
    }
    return Path;
}



void recursiv_copy_to_directory(fs1::path From, fs1::path To, bool keyf, bool All){
    //changeDirectory(From.string());
    for(fs1::directory_iterator rdib(From), rdie; rdib != rdie; ++rdib)
    {
        auto p = rdib->path();

        if(fs1::is_directory(p))
        {
            //changeDirectory(To.string());
            fs1::path newDir = To.string() + "/" + p.filename().string();
            fs1::create_directory(newDir);
            recursiv_copy_to_directory(p, newDir,keyf,All);
        }
        else {
            auto key = CoopyFileToDirectory(p,To,keyf,All);
            if(key){
                keyf = true;
            }
        }
    }
}


int main(int argc, char *argv[]) {

    std::string R = "-R";
    std::string f = "-f";
    bool EXIT = false;
    bool helpKey = false;
    bool regimeKey = false;
    bool lastRegimeR = false;
    bool erorOfR = false;
    bool keyf = false;
    bool All = false;
    std::vector<std::string> DataFromConsol;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            helpKey = true;
        } else if (strcmp(argv[i], "-R") == 0) {
            lastRegimeR = true;
        } else if ((strcmp(argv[i], "-f") == 0)) {
            keyf = true;
        } else {
            DataFromConsol.emplace_back(argv[i]);
        }
    }
    if (helpKey) {
        Help();
        exit(0);
    }

    if(fs1::is_directory(DataFromConsol.back())){
        fs1::path PathTo = FileFinder(DataFromConsol.back());
        for(int i = 0; i < DataFromConsol.size()-1; ++i){
            fs1::path From = DataFromConsol[i];
            if(fs1::is_directory(From)){
                if(lastRegimeR){
                    recursiv_copy_to_directory(From,PathTo,keyf,All);
                }
                else{
                    changeDirectory(PathTo.string());
                    fs1::create_directory(From);
                }
            }
            else{
                All = CoopyFileToDirectory(From,PathTo,keyf,All);
                if(All){
                    keyf = true;
                }
            }
        }
    }
    else{
        if(argc == 3 || argc == 4){
            fs1::path PathToFileRead = FileFinder(DataFromConsol[DataFromConsol.size()-2]);
            if(fs1::is_regular_file(PathToFileRead)){
                std::vector<std::string> Vector = SplitStringToVector(PathToFileRead.filename().string());
                fs1::path PathToFileW = FileFinder(DataFromConsol[DataFromConsol.size()-1]);

                if(fs1::is_regular_file(PathToFileW)){
                    changeDirectory(PathToFileW.parent_path().string());
                    WriteToFile(Vector,PathToFileW.filename().string());
                }
                else if(not fs1::is_directory(PathToFileW) && not fs1::is_regular_file(PathToFileW)){
                    changeDirectory(PathToFileW.parent_path().string());
                    std::fstream fout;
                    fout.open(PathToFileW.filename().string());
                    WriteToFile(Vector,PathToFileW.filename().string());
                    fout.close();
                }
            }
            else{
                std::cout<<"Incorrect argument, you want to read from file that dose not exist !!!"<<std::endl;
                exit(1);
            }
        }
    }
}