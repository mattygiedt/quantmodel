
delete from instrument where symbol = 'CL' and cfi_code <> 'FCMXSX';
delete from instrument where symbol = 'CL' and name not like '%CL CL%';

delete from instrument where symbol = 'GC' and cfi_code <> 'FXXXSX';
delete from instrument where symbol = 'GC' and name not like '%GC GC%';

delete from instrument where symbol = 'SI' and cfi_code <> 'FXXXSX';
delete from instrument where symbol = 'SI' and name not like '%SI SI%';

