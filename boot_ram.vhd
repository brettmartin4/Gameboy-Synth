--+----------------------------------------------------------------------------
--|
--| FILE             : ram_2_port_byte_wr_preload_hex.vhd
--| ORIGINAL AUTHOR  : Patel, Pranav
--| MODIFIED AUTHORS : Dean, James; Martin, Brett
--| RELEASE          : 
--| CREATED          : 01/05/2017
--| UPDATED          : 08/05/2021
--| UDATED           : 05/17/2021
--| 
--+----------------------------------------------------------------------------
--|
--| REQUIRED FILES :
--|
--|    Libraries : ieee
--|    Packages  : std_logic_1164, numeric_std, math_real
--|    Files     : 
--|    Used in   : 
--|
--+----------------------------------------------------------------------------
--|
--| Description :  2 port ram with preload values in hex (ascii) file
--|                *must compile with vhdl-2008 standards*
--|             
--+----------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;
  use ieee.math_real.all;
  use ieee.std_logic_textio.hread;  -- used non-std library because vivado is stupid
library std;
  use std.textio.all;
  
entity boot_ram is
  generic(
          g_byte_width          : integer := 8;
          g_data_num_bytes          : integer := 1;
          g_addr_space_size         : integer := 1024;
          g_ram_data                : string := "../firmware/rv32i_test_seq.rom";          
          g_bram_type               : string := "block"  -- "block" or "distributed" for xilinx, "logic", "M512", "M4K", "M9K", "M144K", "MLAB", or "M-RAM" for altera
         );
  port(
       i_clk_a        : in    std_logic;
       i_clk_en_a     : in    std_logic;
       i_wr_en_a      : in    std_logic_vector(g_data_num_bytes - 1 downto 0);    
       i_addr_a       : in    std_logic_vector(integer(log2(real(g_addr_space_size))) - 1 downto 0);
       i_data_a       : in    std_logic_vector((g_data_num_bytes * g_byte_width) - 1 downto 0);
       o_data_a       :   out std_logic_vector((g_data_num_bytes * g_byte_width) - 1 downto 0);
       i_clk_b        : in    std_logic;
       i_clk_en_b     : in    std_logic;
       i_wr_en_b      : in    std_logic_vector(g_data_num_bytes - 1 downto 0); 
       i_addr_b       : in    std_logic_vector(integer(log2(real(g_addr_space_size))) - 1 downto 0);
       i_data_b       : in    std_logic_vector((g_data_num_bytes * g_byte_width) - 1 downto 0);
       o_data_b       :   out std_logic_vector((g_data_num_bytes * g_byte_width) - 1 downto 0)    
	  );
 attribute ram_style: string;
 attribute ram_style of boot_ram: entity is g_bram_type;    
end entity boot_ram;

architecture rtl of boot_ram is

  type t_ram is array(0 to g_addr_space_size - 1) of std_logic_vector((g_data_num_bytes * g_byte_width) - 1 downto 0);

  impure function initromfromfile (romfilename : in string) return t_ram is
    file romfile           : text open read_mode is romfilename;
    variable v_romfileline : line;
    variable v_rom         : t_ram := (others => (others => '0'));
    variable v_rom_idx    : integer := 0;
  begin
    while not endfile(romfile) loop
      readline (romfile, v_romfileline);
      hread (v_romfileline, v_rom(v_rom_idx));
      v_rom_idx := v_rom_idx + 1;
    end loop;
  return v_rom;
  end function;  
  
  shared variable v_ram    : t_ram := initromfromfile(g_ram_data);

  signal f_data_a : std_logic_vector((g_data_num_bytes * g_byte_width) - 1 downto 0) := (others => '0');
  signal f_data_b : std_logic_vector((g_data_num_bytes * g_byte_width) - 1 downto 0) := (others => '0');
  
begin

  o_data_a <= f_data_a;
  o_data_b <= f_data_b;

  p_read_a: process(i_clk_a) is
  begin
    if rising_edge(i_clk_a) then
      if (i_clk_en_a = '1') then 
        f_data_a <= v_ram(to_integer(unsigned(i_addr_a)));
      end if;
    end if;
  end process p_read_a;
  
  p_write_a: process(i_clk_a) is
  begin
    if rising_edge(i_clk_a) then
      if (i_clk_en_a = '1') then
        for i in 0 to g_data_num_bytes - 1 loop
          if (i_wr_en_a(i) = '1') then
            v_ram(to_integer(unsigned(i_addr_a)))(((i + 1) * g_byte_width - 1) downto (i * g_byte_width)) := i_data_a(((i + 1) * g_byte_width - 1) downto (i * g_byte_width));
          end if;
        end loop;
      end if;
    end if;
  end process p_write_a;

  p_read_b: process(i_clk_b) is
  begin
    if rising_edge(i_clk_b) then
      if (i_clk_en_b = '1') then 
        f_data_b <= v_ram(to_integer(unsigned(i_addr_b)));
      end if;
    end if;
  end process p_read_b;

  p_write_b: process(i_clk_b) is
  begin
    if rising_edge(i_clk_b) then
      if (i_clk_en_b = '1') then
        for i in 0 to g_data_num_bytes - 1 loop
          if (i_wr_en_b(i) = '1') then
            v_ram(to_integer(unsigned(i_addr_b)))(((i + 1) * g_byte_width - 1) downto (i * g_byte_width)) := i_data_b(((i + 1) * g_byte_width - 1) downto (i * g_byte_width));
          end if;
        end loop;
      end if;
    end if;
  end process p_write_b;
  
end architecture rtl; 