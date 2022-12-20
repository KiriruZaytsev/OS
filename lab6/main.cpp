#include <iostream>
#include <csignal>
#include "cstdlib"
#include "Message.h"
#include "ServerNode.h"
#include <sys/wait.h>
#include "SpringBootApplication.h"


int main() {
    SpringBootApplication app;
    app.run();
}
