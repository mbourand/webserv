<img src="https://cdn.discordapp.com/attachments/487745404054536202/850395771307294730/unknown.png">


# Sommaire

1. [Description](#description)
2. [Installation](#installation)
3. [Documentation](#documentation)
   1. [Methodes HTTP Implémentées](#méthodes-http-implémentées)
   2. [Headers HTTP Implémentés](#headers-http-implémentés)
   3. [Configuration](#directives-de-configuration)
   5. [Langues](#langues)


## Description

Webserv est un serveur web minimaliste permettant de répondre à des requêtes HTTP/1.0 et HTTP/1.1.

<img src="https://cdn.discordapp.com/attachments/487745404054536202/851456576505249802/unknown.png">

## Installation

1. ```git clone https://www.github.com/mbourand/webserv && cd webserv```
2. ```sudo apt-get update && sudo apt-get upgrade && sudo apt-get install zlib1g-dev build-essential```
3. ```make```
4. ```./webserv <fichier_de_config>```


# Documentation

## Méthodes HTTP Implémentées

| Méthode | Description                                                       |
|---------|-------------------------------------------------------------------|
| GET     | Renvoie le contenu d'une ressource contenue sur le serveur        |
| HEAD    | Renvoie la même réponse que GET, sans body                        |
| TRACE   | Renvoie le texte brut de la requête dans le body de la réponse    |
| POST    | Envoie des données au serveur, qui seront interprétées par un CGI |
| PUT     | Crée un fichier sur le serveur                                    |
| DELETE  | Supprime un fichier sur le serveur                                |
| OPTIONS | Renvoie les méthodes HTTP implémentées et activées sur le serveur |


## Headers HTTP Implémentés

| Header            | Description                                                                  |
|-------------------|------------------------------------------------------------------------------|
| Accept-Charsets   | Indique les préférences du client quant au charset utilisé dans la réponse   |
| Accept-Encoding   | Indique les préférences du client quant à l'encodage utilisé dans la réponse |
| Accept-Language   | Indique les préférences linguistiques du client                              |
| Allow             | Contient la liste des Méthodes supportées par le serveur à cette adresse     |
| Authorization     | Contient les identifiants encodés en Base64 utilisés pour s'identifier       |
| Content-Language  | Indique la langue du body de la réponse                                      |
| Content-Length    | Indique la taille du body de la réponse                                      |
| Content-Location  | Indique une adresse alternative vers laquelle des données ont été renvoyées  |
| Content-Type      | Indique le type de fichier envoyé dans le body de la réponse                 |
| Date              | Contient la Date locale du serveur                                           |
| Host              | Indique le nom de domaine du serveur et optionnellement le port              |
| Last-Modified     | Indique la date de la dernière modification du fichier                       |
| Location          | Indique l'URL vers laquelle rediriger le client                              |
| Referer           | Indique l'adresse de la page d'origine d'une requête                         |
| Retry-After       | Indique le temps que le client devrait attendre avant de renvoyer une requête|
| Server            | Indique le nom du serveur qui a fourni la réponse                            |
| Transfer-Encoding | Indique l'encodage utilisé pour transférer le corps du message               |
| User-Agent        | Renseigne l'application utilisée par le client, sa version, son OS ...       |
| WWW-Authenticate  | Indique la méthode d'authentification utilisée pour accéder à une ressource  |
| X-*               | Header customisé, qui peut-être envoyé à un CGI                              |


## Directives de Configuration

| Directive            | Scope            | Arguments                         | Description                                                                                      |
|----------------------|------------------|-----------------------------------|--------------------------------------------------------------------------------------------------|
| workers              | global           | workers_amount                    | Définit le nombre de threads lancés en simultané                                                 |
| gzip                 | global           | on/off                            | Active/Désactive la compression gzip                                                             |
| deflate              | global           | on/off                            | Active/Désactive la compression deflate                                                          |
| max_connections      | global           | number                            | Définit le nombre maximum de clients pouvant être connectés sur webserv en même temps            |
| compression_level    | global           | number                            | Définit le niveau de compression [0;9]                                                           |
| server { ... }       | global           |                                   | Indique le début de la configuration d'un serveur                                                |
| server_name          | server           | name1 name2 ...                   | Change le nom du server{}                                                                        |
| listen               | server           | port1 port2 ...                   | Change les ports que le server{} écoute                                                          |
| location             | server           | name { ... }                      | Indique la création d'une location name, qui pourra être utilisée dans un url                    |
| root                 | server, location | path                              | Utilisé dans un location{}, relie le name dans un url au dossier path                            |
| error_page           | server, location | code1 code2 ... path              | Change les pages d'erreur des codes pour le fichier path                                         |
| index                | server, location | path                              | Si une requête est faite sur le name d'une location, le fichier path sera renvoyé par défaut     |
| autoindex            | server, location | on/off                            | Si une requête est faite sur un dossier, l'autoindex liste les fichiers présents dans le dossier |
| max_client_body_size | server, location | size                              | Taille maximale du body d'une requête                                                            |
| disable_methods      | server, location | method1/none method2/none ...     | Désactive des méthodes, "none", réactive toutes les méthodes                                     |
| cgi_dir              | server, location | path                              | Indique le chemin du dossier contenant des exécutables CGI                                       |
| cgi_ext              | server, location | ext path                          | Lance le cgi path si une ressource avec l'extension ext doit être servi                          |
| uploads              | server, location | path                              | Définit le dossier dans lequel les ressources créés par PUT seront créés                         |
| uploads_exts         | server, location | ext1 ext2 ...                     | Définit les extensions qui peuvent être uploadées par PUT                                        |
| auth_basic           | server, location | string                            | Change le nom de la popup d'authentification pour les ressources protégées par un mot de passe   |
| auth_basic_user_file | server, location | path                              | Définit le fichier contenant les logins et mot de passes pour accéder à la ressource             |


#### Exemple simple de configuration

```
# En considérant qu'il existe un dossier www qui contient un fichier index.html
# URL : http://localhost:8080/
server {

	listen 8080
	server_name localhost

	location / {
		root www/
		index index.html
	}

}
```

## Langues

Dans chaque dossier contentant des fichiers, vous pouvez créer un sous-dossier ```.langs``` qui contiendra les variantes des fichiers dans différentes langues.

Dans le dossier .langs, vous pouvez créer des sous-dossiers au nom de la langue dans laquelle seront écrit les fichiers.
Vous pouvez retrouver la liste des langues [ici](https://www.w3schools.com/tags/ref_language_codes.asp).

La bonne langue pour un client est automatiquement déterminée via le header HTTP ```Accept-Language```

#### Arborescence
```
www/
├───.langs/
│   ├───fr/
│   │   ├─── index.html # En français
|   |   └─── ...
│   ├───en-US/
|   |   └─── ... # En anglais des États-Unis
│   ├───en/
|   |   └─── ... # En anglais
│   └───fr-FR/
|       └─── ... # En français de France
└─── index.html # Version originale
```
