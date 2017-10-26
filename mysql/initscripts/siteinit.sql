CREATE DATABASE isle_site CHARACTER SET utf8 COLLATE utf8_general_ci;
CREATE USER islandora_user@'127.0.0.1' IDENTIFIED BY 'islandoraisledb2017';
GRANT SELECT, INSERT, UPDATE ON islandora_docker.* TO 'islandora_user'@'127.0.0.1';
