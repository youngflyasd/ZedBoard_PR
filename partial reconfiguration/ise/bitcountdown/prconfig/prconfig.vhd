----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    19:15:28 10/04/2015 
-- Design Name: 
-- Module Name:    prconfig - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity prconfig is
	Port (clk : in STD_LOGIC;
			reset : in STD_LOGIC;
			input : in STD_LOGIC_VECTOR (7 downto 0);
			output : out STD_LOGIC_VECTOR (7 downto 0));
end prconfig;
	
architecture Behavioral of prconfig is
	signal din,dout : STD_LOGIC_VECTOR(7 downto 0);
begin
	process(clk,reset)
	begin
		if clk'event and clk = '1' then
			din <= input;
			if reset = '0' or din = x"00" then
				din <= x"FF";
				dout <= x"FF";
			else
				dout <= din - 1;
			end if;
		end if;
	end process;
	output <= dout;
end Behavioral;

