#pragma once
#include <string> 
#include <sstream>
#include <vector> 
#include <iostream>


std::string extractBetweenQuotes_user(std::stringstream& ss);

class basic_entry{
    public: 
        std::string email; 
        std::string movie; 
        int rating; 
        std::string date; 
        bool is_valid = false; 


        virtual std::string stringify() const{
            std::stringstream ss; 
            ss  << email   << ", "     
                << movie   << ", "     
                << rating  << ", "     
                << date;
            return ss.str();      
        }

        basic_entry(std::string line){
            std::stringstream ss(line); 
            std::getline( ss, email, ',' );     
            movie = extractBetweenQuotes_user(ss); 
            if(
            ss >> date &&
            ss >> rating  
            ){
                is_valid = true;
            }
            
        }

        

};


class ComEntry :public basic_entry{
    public: 
    std::string email_hashed;
    std::string movie_hashed; 
    
    ComEntry(std::string line) : basic_entry(line){
        
        email_hashed = email; 
        movie_hashed = movie;
        email = movie = "Unknown";
    }
    virtual std::string stringify(){
            
            std::stringstream ss; 
            ss  << "{ "
                << email_hashed << ", "        
                << movie_hashed << ", "        
                << rating       << ", "    
                << date         << " }"    
                ;

            return ss.str(); 
        }
};

class ImdbEntry : public basic_entry{
    public: 
        ImdbEntry(std::string line): basic_entry(line){}

};

std::vector<ComEntry> getComEntries(std::string file_path);
std::vector<ImdbEntry> getImdbEntries(std::string filePath);
std::ostream& operator<<( std::ostream &lhs , const basic_entry  rhs);



template <typename T> 
std::ostream& operator<<(std::ostream& lhs, const std::vector<T> &rhs){
    lhs << "["; 
    for(int i =0; i< rhs.size(); ++i){
        lhs << i<< ": "<<rhs[i] << ", \n"; 
    }
    lhs << "]"; 
    return lhs; 
}
