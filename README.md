## 🛰️ **ft_irc**  

An IRC (Internet Relay Chat) server written in **C++98**, fully compliant with the **RFC 1459** specification.
This project is part of the 42 curriculum and implements a minimal but functional IRC server supporting multiple clients simultaneously using **non-blocking I/O** **and poll()**.  

## 📋 **Overview**  

ft_irc is a simple yet complete IRC server that can handle multiple clients connected via TCP/IP.
It supports standard IRC commands and channel management according to RFC 1459.

You can connect using any IRC client (e.g. **HexChat**) or directly with **netcat** for testing purposes.  

## ⚙️ **Features**  

- **Multi-client handling** using a single poll() loop (no forking, non-blocking I/O)
- **TCP/IP (IPv4/IPv6)** communication
- **User authentication** via password, nickname, and username
- **Channels** with message broadcasting
- **Channel operators and modes**
- Implements the following **IRC commands**:
    - 'PASS', NICK, USER, JOIN, PART, PRIVMSG, QUIT
    - Operator commands:
        - KICK — Remove a client from a channel
        - INVITE — Invite a client to join a channel
        - TOPIC — View or change the channel topic
        - MODE — Set or remove channel modes:
            - i — Invite-only channel
            - t — Restrict topic changes to channel operators
            - k — Channel key (password)
            - o — Give/take operator privilege
            - l — Set user limit

## 🧠 **Technical Details**  

- Language: C++98
- I/O Model: Non-blocking sockets + single poll() call
- Dependencies: None (standard C/C++ and system calls only)
- No forking or multithreading allowed


