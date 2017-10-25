function the_cells = read_cells(fid)
number_of_cells = fread(fid, 1, 'uint64');
cell_identifiers = cell(1, number_of_cells);
cells = cell(1, number_of_cells);

for index = 1:number_of_cells
    cell_identifiers{index} = fread(fid, 1, 'uint64');
    cells{index} = read_cell(fid);
end

the_cells = containers.Map(cell_identifiers, cells);
end
