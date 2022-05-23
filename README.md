<div align="center" id="top"> 
  <img src="./.github/app.gif" alt="Chat app en C" />

  &#xa0;

  <!-- <a href="https://chatenc.netlify.app">Demo</a> -->
</div>

<h1 align="center">Chat En C</h1>

<p align="center">
  <img alt="Github top language" src="https://img.shields.io/github/languages/top/NoNameBoyy/chat-en-c?color=56BEB8">

  <img alt="Github language count" src="https://img.shields.io/github/languages/count/NoNameBoyy/chat-en-c?color=56BEB8">

  <img alt="Repository size" src="https://img.shields.io/github/repo-size/NoNameBoyy/chat-en-c?color=56BEB8">

  <img alt="License" src="https://img.shields.io/github/license/NoNameBoyy/chat-en-c?color=56BEB8">

  <!-- <img alt="Github issues" src="https://img.shields.io/github/issues/{{YOUR_GITHUB_USERNAME}}/chat-en-c?color=56BEB8" /> -->

  <!-- <img alt="Github forks" src="https://img.shields.io/github/forks/{{YOUR_GITHUB_USERNAME}}/chat-en-c?color=56BEB8" /> -->

  <!-- <img alt="Github stars" src="https://img.shields.io/github/stars/{{YOUR_GITHUB_USERNAME}}/chat-en-c?color=56BEB8" /> -->
</p>

<!-- Status -->

<!-- <h4 align="center"> 
	🚧  Chat En C 🚀 Under construction...  🚧
</h4> 

<hr> -->

<p align="center">
  <a href="#dart-about">About</a> &#xa0; | &#xa0; 
  <a href="#sparkles-features">Features</a> &#xa0; | &#xa0;
  <a href="#rocket-technologies">Technologies</a> &#xa0; | &#xa0;
  <a href="#white_check_mark-requirements">Requirements</a> &#xa0; | &#xa0;
  <a href="#checkered_flag-starting">Starting</a> &#xa0; | &#xa0;
  <a href="#memo-license">License</a> &#xa0; | &#xa0;
  <a href="https://github.com/NoNameBoyy" target="_blank">NoNameBoyy</a>
</p>

<br>

## :dart: About ##

Projet de Chat via le terminal en C. Communication au tour à tour commençant par le client. Le client doit posséder un compte pour utiliser le Chat, il lui sera demandé d'en créer un s'il n'en a pas. Un seul Client à la fois.

## :sparkles: Features ##

:heavy_check_mark: Création et gestion de compte via un login + mot de passe\
:heavy_check_mark: Cryptage des messages envoyés dans le modèle du Caesar Cipher\
:heavy_check_mark: Envoie de message en réseau;

## :white_check_mark: Requirements ##

Avant de commencer :checkered_flag:, il est nécessaire d'avoir [Git](https://git-scm.com). Ne fonctionne pas sous Windows.

Le serveur et le client communiquent en local, le bind n'est pas effectué. Pour procéder à une communication entre deux machines sur un même réseau ou sur un réseau différent :

- Insérer l'adresse IP publique dans la fonction inet_addr : <addrServer.sin_addr.s_addr = INADDR_ANY> vers <addrServer.sin_addr.s_addr = inet_addr("164.177.0.0")> (lignes 251 du server et 36 du client);
- Vérifier si les ports sont ouverts, sinon les ouvrir;
- Désactiver le par-feu ou la sécurité nécessaire à la communication réseau (différentes sur windows, mac ou linux);
- Vérifier les subnets pour savoir si une passerelle est nécessaire ou non.


## :checkered_flag: Starting ##

```bash
$ git clone https://github.com/NoNameBoyy/chat-en-c

$ cd chat-en-c

$ gcc -o server server.c

$ ./server

$ gcc -o client client.c

$ ./client
```

## :memo: License ##

This project is under license from MIT. For more details, see the [LICENSE](https://github.com/NoNameBoyy/chat-app-en-c/blob/main/LICENSE) file.


Made with :heart: by <a href="https://github.com/NoNameBoyy" target="_blank">NoNameBoyy </a>


&#xa0;

<a href="#top">Back to top</a>
