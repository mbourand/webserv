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


## Directives de Configuration

| Directive            | Scope            | Arguments                         | Description                                                                                      |
|----------------------|------------------|-----------------------------------|--------------------------------------------------------------------------------------------------|
| workers              | global           | workers_amount                    | Définit le nombre de threads lancés en simultané                                                 |
| gzip                 | global           | on/off                            | Active/Désactive la compression gzip                                                             |
| deflate              | global           | on/off                            | Active/Désactive la compression deflate                                                          |
| max_connections      | global           | number                            | Définit le nombre maximum de clients pouvant être connectés sur webserv en même temps            |
| compression_level    | global           | number                            | Définit le niveau de compression [0;9]                                                           |
| server { ... }       | global           |                                   | Indique le début de la configuration d'un serveur                                                |
| server_name          | server           | name                              | Change le nom du server{}                                                                        |
| listen               | server           | port1 port2 ...                   | Change les ports que le server{} écoute                                                          |
| location             | server           | name { ... }                      | Indique la création d'une location name, qui pourra être utilisée dans un url                    |
| root                 | server, location | path                              | Utilisé dans un location{}, relie le name dans un url au dossier path                            |
| error_page           | server, location | code1 code2 ... path              | Change les pages d'erreur des codes pour le fichier path                                         |
| index                | server, location | path                              | Si une requête est faite sur le name d'une location, le fichier path sera renvoyé par défaut     |
| autoindex            | server, location | on/off                            | Si une requête est faite sur un dossier, l'autoindex liste les fichiers présents dans le dossier |
| max_client_body_size | server, location | size                              | Taille maximale du body d'une requête                                                            |
| disable_methods      | server, location | method1/none method2/none ...     | Désactive des méthodes, "none", réactive toutes les méthodes                                     |
| cgi_dir              | server, location | path                              |                                                                                                  |
| cgi_ext              | server, location | ext path                          | Lance le cgi path si une ressource avec l'extension ext doit être servi                          |
| uploads              | server, location | path                              | Définit le dossier dans lequel les ressources créés par PUT seront créés                         |
| uploads_exts         | server, location | ext1 ext2 ...                     | Définit les extensions qui peuvent être uploadées par PUT                                        |
| auth_basic           | server, location | string                            | Change le nom de la popup d'authentification pour les ressources protégées par un mot de passe   |
| auth_basic_user_file | server, location | path                              | Définit le fichier contenant les logins et mot de passes pour accéder à la ressource             |
