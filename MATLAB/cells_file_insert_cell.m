function cells_file_insert_cell(name, cell_identifier, the_cell, ~)
if nargin >= 4
    exclusive_mode = varargin{3};
else
    exclusive_mode = true;
end

fid = fopen(name);
the_cells = read_cells(fid);
fclose(fid);

if exclusive_mode && isKey(the_cells, cell_identifier)
    error([ ...
        'The cells file `', ...
        name, ...
        ''' contains a cell, the identifier of which is `', ...
        num2str(cell_identifier), ...
        ''', which is the same as cell_identifier']);
end

cell_identifiers = [keys(the_cells), cell_identifier];
cells = [values(the_cells), the_cell];
fopen(name, 'w');
write_cells(fid, containers.Map(cell_identifiers, cells));
end
