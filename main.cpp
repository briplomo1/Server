#include <iostream>
#include "socket/SocketArgs.h"
#include "socket/SSocket.h"


// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    constexpr SOCK_FAM fam = IPV4;
    constexpr SOCK_TYPE sockType = TCP;
    const auto args = SocketArgs(fam, sockType);
    //auto sock = SSocket(args);
    //sock.listen();

    return 0;
}
// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.