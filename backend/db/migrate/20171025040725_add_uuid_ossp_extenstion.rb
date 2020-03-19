class AddUuidOsspExtenstion < ActiveRecord::Migration[6.0]
  def up
    enable_extension "uuid-ossp"
  end

  def down
    disable_extension "uuid-ossp"
  end
end
