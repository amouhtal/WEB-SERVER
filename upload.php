<?php
    foreach ($_FILES as $key => $value) {
  
    if ($_FILES[$key]['error']) {
        switch ($_FILES[$key]['error']) {
            case 1: // UPLOAD_ERR_INI_SIZE
                echo "Le fichier dépasse la taille autorisée par le serveur";
                break;
              
            case 2: // UPLOAD_ERR_FORM_SIZE
                echo "Le fichier dépasse la limite autorisée dans le formulaire HTML";
                break;
  
            case 3: // UPLOAD_ERR_PARTIAL
                echo "L'envoi du fichier à été intérrompu pendant le transfert";
                break;
            case 4: // UPLOAD_ERR_NO_FILE
                echo "Le fichier que vous avez uploadé a une taille nulle";
                break;
        }
    } else {
        if ((isset($_FILES[$key]['tmp_name'])&&($_FILES[$key]['error'] == UPLOAD_ERR_OK))) {    
            $name = $_FILES[$key]['name'];    
            echo $name;
            $dir = './upload/';
            move_uploaded_file($_FILES[$key]['tmp_name'], $dir . "$name");
            echo "L'image à été uploadé !";  
        } else {
            echo "L'image n'as pas été uploadé";
        }
    }
 }
?>