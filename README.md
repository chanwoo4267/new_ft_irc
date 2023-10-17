<a name="readme-top"></a>

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]

<br />
<div align="center">

<h3 align="center">ft_irc</h3>

  <p align="center">
    This project is about creating your own IRC server.
    <br />
    <a href="https://cdn.intra.42.fr/pdf/pdf/102841/en.subject.pdf"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/chanwoo4267/new_ft_irc/issues">Report Bug</a>
    ·
    <a href="https://github.com/chanwoo4267/new_ft_irc/issues">Request Feature</a>
  </p>
</div>

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

[![Product Name Screen Shot][product-screenshot]]() <!-- ?? -->

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With

* [![C++][Cpp-shield]][Cpp-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

This is an example of how you may give instructions on setting up your project locally.
To get a local copy up and running follow these simple example steps.

### Prerequisites

* C++98
* irssi
```
pip install irssi
```

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/chanwoo4267/new_ft_irc
   ```
3. Compile source codes
   ```sh
   make all
   ```


<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

1. execute executable file with port and password
   ```js
   ./ircserver 5551 1234
   ```
2. join server with irssi client with IP, port, password
    ```
    ./irssi -c 127.0.0.1 -p 5551 -w 1234
    ```

Implemented features at client:

* join channel
```
/join #channel_name
```

* send message
```
/privmsg #channel_name message
/privmsg user_nick message
```

* change channel mode
```
/mode #channel_name [+/-][i/t/k/l/o] [arguments]
```

 * invite user to channel
```
/invite user_nick #channel_name
```

* show and set channel topic
```
/topic
/topic #channel_name topic_content
```

* kick user from channel
```
/kick user_nick #channel_name
```

* change nickname
```
/nick nickname
```

* change user info
```
/user username servername hostname :realname
```

* terminate
```
/exit
```

_For more examples, please refer to the [Irssi Document](https://irssi.org/)_

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* [Chirc](http://chi.cs.uchicago.edu/chirc/irc.html)
* [Connection Between Client and Server](https://beej.us/guide/bgnet/pdf/bgnet_a4_c_1.pdf)
* [IRC client protocols](https://modern.ircdocs.horse/)
* [IRSSI](https://irssi.org/)
* [Notion](https://obtainable-poison-b83.notion.site/ft_irc-bc927002ba7a4d6893388ffefb2d24f5?pvs=4)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/chanwoo4267/new_ft_irc.svg?style=for-the-badge
[contributors-url]: https://github.com/chanwoo4267/new_ft_irc/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/chanwoo4267/new_ft_irc.svg?style=for-the-badge
[forks-url]: https://github.com/chanwoo4267/new_ft_irc/network/members

[C-shield]: https://img.shields.io/badge/C-35495E?style=for-the-badge&logo=c&logoColor=White
[C-url]: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2731.pdf

[Cpp-shield]: https://img.shields.io/badge/C++-35495E?style=for-the-badge&logo=c++&logoColor=White
[Cpp-url]: https://www.open-std.org/jtc1/sc22/wg21/

[product-screenshot]: image/new_ft_irc.png