function corr_val = Find_val(name,structure)

%The function finds the corresponding numeric variables for the given name/ 
%column header in a structure

col_num = strcmp( name , structure.textdata); % Alt: structure.colheaders
corr_val = structure.data(:,col_num);

end