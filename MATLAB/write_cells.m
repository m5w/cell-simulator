function write_cells(fid, the_cells)
fwrite(fid, length(the_cells), 'uint64');

for value = [keys(the_cells); values(the_cells)]
    fwrite(fid, value{1}, 'uint64');
    write_cell(fid, value{2});
end
end
