class CreateEvents < ActiveRecord::Migration[6.0]
  def change
    create_table :events, id: false do |t|
      t.uuid :id, null: false, primary_key: true, default: 'uuid_generate_v4()'
      t.string :parser
      t.string :partition
      t.string :topic
      t.string :offset
      t.string :event
      t.uuid :uuid
      t.jsonb :data
      t.datetime :create_time, default: -> { 'CURRENT_TIMESTAMP' }
      t.datetime :receive_time, default: -> { 'CURRENT_TIMESTAMP' }
    end
  end
end
