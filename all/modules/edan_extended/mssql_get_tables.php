<?php
/**
 * SQL PHP Example
 * 
 * This script will list all the tables in the specified data source.
 */

ini_set('memory_limit', '-1');

$data_source = 'odbc:DRIVER={Easysoft ODBC-SQL Server};SERVER=23.96.212.165;UID=aaadev;PWD=aaadev;DATABASE=AAADigitalCollections';

$sql = "SELECT * FROM information_schema.tables";

try {
  // Connect to the data source and get a database handle for that connection.
  $dbh = new PDO($data_source);
  $stmt = $dbh->query($sql);
  $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
  $data['error'] = $dbh->errorInfo();
} catch (PDOException $e) {
   $data['error'] = 'Failed to connect: ' . $e->getMessage();
}

var_dump($data);

die();