function write_cell_discharge_curve( ...
    fid, ...
    charge_discharged_from_cell, ...
    voltage_source_voltage, ...
    mean_internal_conductance, ...
    electric_potential_energy)
fwrite(fid, numel(charge_discharged_from_cell), 'uint64');
fwrite( ...
    fid, ...
    [ ...
    charge_discharged_from_cell'; ...
    voltage_source_voltage'; ...
    electric_potential_energy'], ...
    'double');
fwrite(fid, mean_internal_conductance, 'double');
end
