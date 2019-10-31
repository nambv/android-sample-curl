//
// Created by Anmol Verma on 5/1/17.
//

#ifndef ANDROID_APIS_H
#define ANDROID_APIS_H

#include <stdio.h>
#include <string>

class NetworkService {
private:
    const char *accessToken;
    const char *deviceId;
    const char *deviceType;
    const char *locale;
    const char *appVersion;

protected:

    NetworkService(const char *accessToken,
                   const char *deviceId,
                   const char *deviceType,
                   const char *locale,
                   const char *appVersion);

    std::string registerKey();

    std::string renewKey();

    std::string trackingOnline();
};

std::string loginApi(const char *email, const char *password);


#endif //ANDROID_APIS_H
