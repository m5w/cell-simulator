function the_cell = read_cell(fid)
number_of_points = fread(fid, 1, 'uint64');
discharge_curve = ...
    reshape( ...
        fread(fid, 3 * number_of_points, 'double'), 3, number_of_points);
mean_internal_conductance = fread(fid, 1, 'double');
charge_supplied = discharge_curve(1, :)';
open_circuit_voltage = discharge_curve(2, :)';
work = discharge_curve(3, :)';
the_cell = ...
    make_cell( ...
        charge_supplied, ...
        open_circuit_voltage, ...
        mean_internal_conductance, ...
        work);
end