<?php
$target_dir = "upload/";
print_r($_FILES);
$target_file = $target_dir . basename($_FILES["video"]["name"]);
$uploadOk = 1;
$imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));
// Check if image file is a actual image or fake image
echo "hello";
    move_uploaded_file($_FILES["video"]["name"], $target_file);
?>