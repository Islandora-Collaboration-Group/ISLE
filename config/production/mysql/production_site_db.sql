CREATE DATABASE enduser_institution_drupal_site_db CHARACTER SET utf8 COLLATE utf8_general_ci;
CREATE USER 'enduser_institution_drupal_site_db_user'@'%' IDENTIFIED BY 'enduser_institution_drupal_site_db_pw';
GRANT SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, INDEX, ALTER, ALTER ROUTINE, CREATE ROUTINE, CREATE TEMPORARY TABLES, CREATE VIEW, EVENT, EXECUTE, LOCK TABLES, REFERENCES, SHOW VIEW, TRIGGER ON enduser_institution_drupal_site_db.* TO 'enduser_institution_drupal_site_db_user'@'%';
FLUSH PRIVILEGES;
