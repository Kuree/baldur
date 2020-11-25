module top (
    input logic in,
    output logic out
);

logic a, b;

always_comb begin
    a = in;
    b = in;
    out = a + b;
end

endmodule
