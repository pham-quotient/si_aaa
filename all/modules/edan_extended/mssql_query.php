<?php
/**
 * SQL PHP Example
 * 
 * This script will retrieve 10 Collection records from the database.
 */

ini_set('memory_limit', '-1');

$data = array();
$db_args = array();
$conn = false;
$stmt = false;

// How to handle MS Access MDB files in Linux with PHP5 PDO and ODBC
// https://gist.github.com/amirkdv/9672857

$data_source = 'odbc:DRIVER={Easysoft ODBC-SQL Server};SERVER=23.96.212.165;UID=aaadev;PWD=aaadev;DATABASE=AAADigitalCollections';

// $sql = "SELECT TOP 10 
//   CollectionID,
//   CollectionTitle
// FROM dbo.tblCollection";

$sql = "SELECT 
      ISNULL(InstitutionName, '') as InstitutionName 
      FROM dbo.tblInstitution
      WHERE InstitutionType = 'A' 
      OR InstitutionType = 'a' 
      OR (InstitutionType = 'v' AND InstitutionXREF = 1) 
      OR InstitutionType = 'x' 
      OR ((InstitutionType IS NOT NULL AND InstitutionType = '') AND (InstitutionXREF = 1 OR InstitutionXREF = 0))";

// $sql = "SELECT DISTINCT 
//     ISNULL(PersonLName, '') as LastName, 
//     ISNULL(PersonFName, '') as FirstName, 
//     ISNULL(DisplayDateBorn, '') as DateBorn,
//     ISNULL(DisplayDateDied, '') as DateDied,
//     CONCAT(ISNULL(PersonLName, ''), ', ', ISNULL(PersonFName, ''), ', ', ISNULL(DisplayDateBorn, ''), ' - ', ISNULL(DisplayDateDied, '')) as DisplayName
//     FROM dbo.tblPerson";

    // WHERE (PersonNameType IS NOT NULL && PersonNameType = 'v') 
    // OR (PersonNameType IS NOT NULL && PersonNameType = 'x') 
    // OR (PersonNameType IS NOT NULL && AND PersonXREF = '0')

try {
  // Connect to the data source and get a database handle for that connection.
  $dbh = new PDO($data_source);
  $stmt = $dbh->prepare($sql);
  $stmt->execute();
  $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
  $data['error'] = $dbh->errorInfo();
} catch (PDOException $e) {
   $data['error'] = 'Failed to connect: ' . $e->getMessage();
}

var_dump($data);

die();