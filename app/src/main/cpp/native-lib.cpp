#include <jni.h>
#include <string>
#include <curl.h>
#include "network-service.h"
#include <iostream>
#include <map>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_consciousapps_cmakecurltest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */, jstring email, jstring password) {

    const char *emailChar = env->GetStringUTFChars(email, nullptr);
    const char *passwordChar = env->GetStringUTFChars(password, nullptr);

    std::string response = loginApi(emailChar, passwordChar);
    return env->NewStringUTF(response.c_str());
}


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

std::string loginApi(const char *email, const char *password) {

    std::string readBuffer;
    int responseCode = 0;

    char bodyContent[1000];
    strcpy(bodyContent, R"({ "email" : ")");
    strcat(bodyContent, email);
    strcat(bodyContent, R"(", "password" : "})");
    strcat(bodyContent, password);
    strcat(bodyContent, R"("})");

    CURL *curl;
    CURLcode res;

    char *jsonBody = const_cast<char *>(R"({ "email" : "eve.holt@reqres.in" , "password" : "cityslicka" })");

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://reqres.in/api/login");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bodyContent);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        } else {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    std::cout << readBuffer << std::endl;
    std::cout << responseCode << std::endl;

    return "Response Code: "
           + std::to_string(responseCode)
           + "\n"
           + "Response Body: "
           + readBuffer;
}

int submitCUrlRequest(std::map<char *, char *> headers, std::map<char *, char *> content) {

    CURL *curl;
    CURLcode res;

    struct curl_slist *requestHeaders = NULL;
    std::string readBuffer;
    int responseCode = 0;

    // Build headers request
    std::map<char *, char *>::iterator itr;
    for (itr = headers.begin(); itr != headers.end(); ++itr) {
        std::cout << '\t' << itr->first
                  << '\t' << itr->second << '\n';
        char header[1000];
        strcpy(header, itr->first);
        strcat(header, itr->second);
        requestHeaders = curl_slist_append(requestHeaders, header);
    }

    // Build body request
    char bodyContent[1000];
    strcpy(bodyContent, R"({)");

    if (!content.empty()) {
        std::map<char *, char *>::iterator dataItr;
        for (dataItr = content.begin(); dataItr != content.end(); ++dataItr) {
            strcpy(bodyContent, R"(")");
            strcpy(bodyContent, dataItr->first);
            strcpy(bodyContent, R"(":")");
            strcat(bodyContent, dataItr->second);
            strcat(bodyContent, R"(")");
            strcat(bodyContent, R"(,)");
        }
    }

    strcat(bodyContent, R"(})");

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://q7dmz.mocklab.io/renew_key");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, requestHeaders);

        if (!content.empty()) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bodyContent);
        }

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        } else {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    return responseCode;
}

int NetworkService::registerKey(std::map<char *, char *> headers) {
    std::map<char *, char *> content;
    return submitCUrlRequest(headers, content);
}

int NetworkService::renewKey(std::map<char *, char *> headers, std::map<char *, char *> content) {
    return submitCUrlRequest(headers, content);
}

int NetworkService::trackingOnline(std::map<char *, char *> headers,
                                           std::map<char *, char *> content) {
    return submitCUrlRequest(headers, content);
}