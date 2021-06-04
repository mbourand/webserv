# webserv

<img src="https://cdn.discordapp.com/attachments/487745404054536202/850395771307294730/unknown.png">


## Description

Webserv est un serveur web minimaliste permettant de répondre à des requêtes HTTP/1.0 et HTTP/1.1.


## Méthodes HTTP implémentées

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
| Allow             |                                                                              |
| Authorization     |                                                                              |
| Content-Language  | Indique la langue du body de la réponse                                      |
| Content-Length    | Indique la taille du body de la réponse                                      |
| Content-Location  |                                                                              |
| Content-Type      | Indique le type de fichier envoyé dans le body de la réponse                 |
| Date              |                                                                              |
| Host              |                                                                              |
| Last-Modified     |                                                                              |
| Location          |                                                                              |
| Referer           |                                                                              |
| Retry-After       |                                                                              |
| Server            | Indique le nom du serveur qui a fourni la réponse                            |
| Transfer-Encoding |                                                                              |
| User-Agent        |                                                                              |
| WWW-Authenticate  |                                                                              |
| X-*               | Header customisé, qui peut-être envoyé à un CGI                              |
