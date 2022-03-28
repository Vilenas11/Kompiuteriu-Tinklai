#include "mainHeader.h"
#ifdef _WIN32
#include <winsock2.h>
#define socklen_t int
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include<time.h>
#include <vector>
#include <stdbool.h>
class Question{
private:
    std::string testQuestion;
    std::vector<std::string> questionAnswers;
    int correctAnswer;
public:
    Question(){
        testQuestion = "";
    }
    void set_testQuestion(std::string testQuestion1){
        testQuestion = testQuestion1;

        }
    void checkForCorrectAnswer(){
        for(int i=0; i<questionAnswers.size(); i++){
            if(questionAnswers[i][0] == '!'){
                questionAnswers[i].erase(0,1);
                correctAnswer = i;
            }
        }
    }
    void set_questionAnswersVector(std::vector<std::string> lol){
        questionAnswers = lol;
        checkForCorrectAnswer();

    }
    void set_questionAnswers(std::string string1, std::string string2, std::string string3){
        if(string1[0] == '!'){
            string1.erase(0,1);
            correctAnswer = 0;
            questionAnswers.push_back(string1);
            questionAnswers.push_back(string2);
            questionAnswers.push_back(string3);
        }
        else if(string2[0] == '!'){
            string2.erase(0,1);
            correctAnswer = 1;
            questionAnswers.push_back(string1);
            questionAnswers.push_back(string2);
            questionAnswers.push_back(string3);
        }
        else{
            string3.erase(0,1);
            correctAnswer=2;
            questionAnswers.push_back(string1);
            questionAnswers.push_back(string2);
            questionAnswers.push_back(string3);
        }
    }
    std::string get_testQuestion(){
        return testQuestion;
    }
    std::vector<std::string> get_questionAnswers(){
        return questionAnswers;
    }
    int get_AnswerNumber(){
        return questionAnswers.size();
    }
    int get_correctAnswer(){
        return correctAnswer;
    }
    //get_testQuestion
        Question& operator=(Question other)
    {
        testQuestion = other.get_testQuestion();
        questionAnswers = other.get_questionAnswers();
        correctAnswer = other.get_correctAnswer();
        return *this;
    }
};

class Quiz{
private:
    std::vector<Question> questions;
public:
    void set_TestQuestions(){
        Question tempObject;
        int i = 0, j=0;
        std::vector<std::string> tempVector;
        tempVector = tempObject.get_questionAnswers();
        std::ifstream file("Questions.txt");
        std::string str;
        while (std::getline(file, str)){
            if(tempObject.get_testQuestion().empty()){
                tempObject.set_testQuestion(str);
            }
            else if(tempVector.size() == 0){
                tempVector.push_back(str);
            }
            else if(tempVector.size() == 1){
                tempVector.push_back(str);
            }
            else{
                tempVector.push_back(str);
            }
            j++;
            if(j % 4 == 0){
                tempObject.set_questionAnswersVector(tempVector);
                questions.push_back(tempObject);
                i++;
                tempObject.set_testQuestion("");
                tempVector.clear();
                tempObject.set_questionAnswersVector(tempVector);
            }

        }

    }
    void coutEverything(){
        for(int i=0; i<questions.size(); i++){
            std::cout<<questions[i].get_testQuestion()<<std::endl;
             std::vector<std::string> temporary = questions[i].get_questionAnswers();
            for(int j =0; j<3; j++){
                std::cout<<temporary[j]<<std::endl;

            }
            std::cout<<"CORRECT ANSWER: "<<questions[i].get_correctAnswer()<<std::endl;
        }
    }
    int randomNumberGenerator(){
        srand(time(0));
        int random_integer = rand()%questions.size();
        return random_integer;
    }
    /*Question pickARandomQuestion(){
        int number = randomNumberGenerator();
        return questions[number];
    }*/
    Question coutARandomQuestion(){
        int number = randomNumberGenerator();
        /*std::cout<<questions[number].get_testQuestion()<<std::endl;
        for(int i=0; i<questions[number].get_questionAnswers().size(); i++){
            std::cout<<questions[number].get_questionAnswers()[i]<<std::endl;
        }*/
        return questions[number];
    }
    bool checkForCorrectAnswer(char hello[], Question tempp){
        char temp[2];
        for(int i=0; i<tempp.get_AnswerNumber(); i++)
            if(tempp.get_correctAnswer() == i)
               // strcpy(temp)
               temp[0] = 65+i;
               temp[1] = '\0';


        if(strcmp(hello, temp )== 0) return true;
        else return false;
    }

};
void funkcija(int clientSocket);

int main(int argc, char *argv []){
#ifdef _WIN32
    WSADATA data;
#endif
    unsigned int port;
    int l_socket;
    int c_socket; 
    struct sockaddr_in servaddr; //Server adress structure
    struct sockaddr_in clientaddr; //client adress structure
//    int clientaddrlen;
    socklen_t clientaddrlen;

    int s_len;
    int r_len;
    
    if (argc != 2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[1]);

    if ((port < 1) || (port > 65535)){
        printf("ERROR #1: invalid port specified.\n");
        exit(1);
    }

#ifdef _WIN32
    WSAStartup(MAKEWORD(2,2),&data);    
#endif

    if ((l_socket = socket(AF_INET, SOCK_STREAM,0))< 0){ //Server socket is created 
        fprintf(stderr,"ERROR #2: cannot create listening socket.\n");
        exit(1);
    }
    
    memset(&servaddr,0, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; //IP adress is assigned
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(port); // port is assigned

    if (bind (l_socket, (struct sockaddr *)&servaddr,sizeof(servaddr))<0){  //Server adress is connected with a socket
        fprintf(stderr,"ERROR #3: bind listening socket.\n");
        exit(1);
    }

    if (listen(l_socket, 5) <0){  //The socket is waiting a client connection
        fprintf(stderr,"ERROR #4: error in listen().\n");
        exit(1);
    }
    else fprintf(stderr, "Server is waiting for the client \n");

        memset(&clientaddr,0, sizeof(clientaddr)); //client adress is nulled

        clientaddrlen = sizeof(struct sockaddr);
        if ((c_socket = accept(l_socket,
            (struct sockaddr*)&clientaddr,&clientaddrlen))<0){
            fprintf(stderr,"ERROR #5: error occured accepting connection.\n");
            exit(1);
        }
        std::cout<<"The server has succesfully established a connection"<<std::endl;
        funkcija(c_socket);
    return 0;
}
void funkcija(int clientSocket){
    char buffer[1024];
    Quiz a;
    Question temporaryObject;
    std::string tempString;
    a.set_TestQuestions();
    int correctlyAnsweredQuestions = 0;
    std::vector<std::string> temporaryVector;
    char endLine = '\n';
    //a.coutEverything();
    //a.coutARandomQuestion();
    int k, r;
    int laik =0;
    for(int j=0; j<5; j++){
        //if(j!=5){
       // printf("LOL");
        memset(&buffer,0,sizeof(buffer));
        temporaryObject = a.coutARandomQuestion();
        tempString = temporaryObject.get_testQuestion();
        tempString += endLine;
        for(int i = 0; i<temporaryObject.get_questionAnswers().size(); i++){
            temporaryVector.push_back(temporaryObject.get_questionAnswers()[i]);
            tempString += 65+i;
            tempString += ')';
            tempString += temporaryVector[i];
            tempString += endLine;
        }
        temporaryVector.clear();
        strcpy(buffer, tempString.c_str());
        r = send(clientSocket, buffer , strlen(buffer), 0);
        /*if(r == 0 ){
            std::cout<<"THE QUESTION WAS SUCCESFULLY SENT"<<std::endl;

        }*/
        //std::cout<<temporaryString<<std::endl;
        //std::cout<<"BUFFER LENGTH2: "<<strlen(buffer)<<std::endl;
       // r = send(clientSocket, buffer , strlen(buffer), 0);
        //std::cout<<"ilgis: "<<strlen(buffer)<<std::endl;
        /*for(int i=0; i<temporaryObject.get_questionAnswers().size(); i++){
            printf("lol:");
            memset(&buffer,0,sizeof(buffer));
            temporaryString = temporaryObject.get_questionAnswers()[i];
            //std::cout<<temporaryString<<std::endl;
            strcpy(buffer, temporaryString.c_str());
            r = send(clientSocket, buffer , strlen(buffer), 0);
        }*/
        
        memset(&buffer,0,sizeof(buffer));
        k = recv(clientSocket,buffer,sizeof(buffer),0);
        buffer[strlen(buffer)-1] = '\0';
        printf("%s,%d\n", buffer, k);
       if(a.checkForCorrectAnswer(buffer,temporaryObject) == true){
           std::string tempo = "YOUR ANSWER WAS CORRECT!";
           correctlyAnsweredQuestions++;
           memset(&buffer,0,sizeof(buffer));
           tempo += '\n';
           strcpy(buffer, tempo.c_str());
           r = send(clientSocket, buffer , strlen(buffer), 0);
         }
        else{
            memset(&buffer,0,sizeof(buffer));
            std::string tempo = "YOUR ANSWER WAS INCORRECT!";
            tempo += '\n';
            strcpy(buffer, tempo.c_str());
            r = send(clientSocket, buffer , strlen(buffer), 0);
        }

        if(k == -1) {
            printf("The client has disconnected\n");
            close(clientSocket);
            printf("Closing the server");
            break;
            }
        }
        /*else {
            memset(&buffer,0,sizeof(buffer));
            std::string tempo = "You have answered ";
            tempo += std::to_string(correctlyAnsweredQuestions);
            tempo += " Questions correctly!\n";
            std::cout<<tempo;
            strcpy(buffer, tempo.c_str());
            r = send(clientSocket, buffer , strlen(buffer), 0);
         }*/
    

}