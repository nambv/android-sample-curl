//
// Created by Anmol Verma on 5/1/17.
//

#ifndef ANDROID_APIS_H
#define ANDROID_APIS_H

#include <stdio.h>
#include <string>
#include <map>
#include <curl.h>

class NetworkService {

public:

    int registerKey(std::map<char *, char *> headers);

    int renewKey(std::map<char *, char *> headers, const std::map<char *, char *> data);

    int trackingOnline(std::map<char *, char *> headers, std::map<char *, char *> data);
};

std::string loginApi(const char *email, const char *password);


#endif //ANDROID_APIS_H
