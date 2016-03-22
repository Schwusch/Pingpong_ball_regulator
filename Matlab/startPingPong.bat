set currentPath=%~dp0
matlab -nosplash -nodesktop -minimize -r cd('%currentPath%');pingPong;