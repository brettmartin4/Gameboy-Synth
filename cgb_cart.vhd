--+----------------------------------------------------------------------------
--|
--| FILE             : cgb_cart.vhd
--| ORIGINAL AUTHOR  : Brett Martin
--| HDL Version      : VHDL - 2008
--|
--+----------------------------------------------------------------------------
--|
--| Description :  Gameboy Color cartridge emulator
--|
--|   Pinout:
--| 32 - Ground
--| 31 - Audio in
--| 30 - Reset (active low)
--| 29 downto 22 - Data bus
--| 21 downto 6 - Address bus
--| 5 - Chip Select (active low)
--| 4 - Read signal (active low)
--| 3 - Write signal (active low)
--| 2 - Clock
--| 1 - Vcc (+5V)
--|
--| Gameboy EPROM is 32kB*8
--|
-------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity cgb_cart is
    port (
        clk : in std_logic;
        rst : in std_logic; -- Not used
        wr : in std_logic;
        rd : in std_logic;
        cs : in std_logic; -- Not used
        audio : in std_logic; -- Not used
        addr : in std_logic_vector(15 downto 0);
        data : out std_logic_vector(7 downto 0)
    );
end cgb_cart;

architecture rtl of cgb_cart is

    signal channel_sel : std_logic(3 downto 0);

    -- Global
    signal reg_volume : std_logic_vector(10 downto 0);
    signal reg_len_load : std_logic_vector(10 downto 0);
    signal reg_trigger : std_logic;
    signal reg_len_en : std_logic;

    -- Channels 1-3
    signal reg_env_add : std_logic;
    signal reg_period : std_logic_vector(10 downto 0);

    -- Channels 1-2
    signal reg_duty : std_logic_vector(10 downto 0);

    -- Square Channel
    signal reg_sweep_pad : std_logic_vector(10 downto 0);
    signal reg_negate : std_logic;
    signal reg_shift : std_logic_vector(10 downto 0);

    -- Noise Channel
    signal reg_clk_shift : std_logic_vector(10 downto 0);
    signal reg_lfsr_width : std_logic;
    signal reg_div_code : std_logic_vector(10 downto 0);

begin

    --process(clk)

end architecture;